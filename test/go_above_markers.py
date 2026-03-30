#!/usr/bin/env python3
import time

import rclpy
from geometry_msgs.msg import PoseStamped

from scene_objects_manager.srv import DetectBlocksPoses
from aegis_director.robot_director import RobotDirector


def main():
    rclpy.init()

    director = RobotDirector(synchronous=True)

    NUM_OF_ITER = 1

    for i in range(NUM_OF_ITER):
        director.joint_move(
            joint_positions={
                "shoulder_pan_joint": 0.0,
                "shoulder_lift_joint": -2.09,
                "elbow_joint": 2.09,
                "wrist_1_joint": -1.57,
                "wrist_2_joint": -1.57,
                "wrist_3_joint": 0.0,
            },
            max_vel=0.1,
            max_accel=0.1,
        )

        time.sleep(2.0)

        service_name = "/detect_blocks_poses"
        target_frame_fallback = "base_link"
        z_offset = 0.005
        wait_at_target_s = 3.0

        cli = director.node.create_client(DetectBlocksPoses, service_name)
        director.node.get_logger().info(f"Waiting for service {service_name} ...")
        if not cli.wait_for_service(timeout_sec=10.0):
            director.node.get_logger().error(f"Service {service_name} not available.")
            director.node.destroy_node()
            rclpy.shutdown()
            return

        future = cli.call_async()
        rclpy.spin_until_future_complete(director.node, future, timeout_sec=10.0)

        resp = future.result()
        if resp is None:
            director.node.get_logger().error("Service call failed / timed out.")
            director.node.destroy_node()
            rclpy.shutdown()
            return

        pose_array = resp.poses
        frame_id = pose_array.header.frame_id or target_frame_fallback

        director.node.get_logger().info(
            f"Got {len(pose_array.poses)} pose(s) in frame '{frame_id}'."
        )

        targets: list[PoseStamped] = []
        for i, p in enumerate(pose_array.poses):
            target = PoseStamped()
            target.header.stamp = director.node.get_clock().now().to_msg()
            target.header.frame_id = frame_id
            target.pose = p
            target.pose.position.z += z_offset

            targets.append(target)

            director.node.get_logger().info(
                f"[{i + 1}] above: x={target.pose.position.x:.3f}, "
                f"y={target.pose.position.y:.3f}, z={target.pose.position.z:.3f}"
            )

        for i, t in enumerate(targets):
            director.node.get_logger().info(
                f"Moving to target {i + 1}/{len(targets)} ..."
            )
            director.pose_move(
                pose=t,
                cartesian=False,
                max_vel=0.1,
                max_accel=0.1,
            )
            director.node.get_logger().info(
                f"Reached target {i + 1}. Waiting {wait_at_target_s:.1f}s ..."
            )
            time.sleep(wait_at_target_s)

    director.node.get_logger().info("Done with all targets.")
    director.node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
