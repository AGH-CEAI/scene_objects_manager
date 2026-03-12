#!/usr/bin/env python3
import rclpy

from geometry_msgs.msg import PoseStamped, TransformStamped
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster

from scene_objects_manager.srv import DetectBlocksPoses
from aegis_director.robot_director import RobotDirector


def main():
    rclpy.init()

    director = RobotDirector(synchronous=True)

    service_name = "/detect_blocks_poses"
    target_frame_fallback = "base_link"
    z_offset = 0.10

    cli = director.node.create_client(DetectBlocksPoses, service_name)
    static_broadcaster = StaticTransformBroadcaster(director.node)

    director.node.get_logger().info(f"Waiting for service {service_name} ...")
    if not cli.wait_for_service(timeout_sec=10.0):
        director.node.get_logger().error(f"Service {service_name} not available.")
        director.node.destroy_node()
        rclpy.shutdown()
        return

    req = DetectBlocksPoses.Request()
    req.detect = True

    future = cli.call_async(req)
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
    static_transforms: list[TransformStamped] = []

    now = director.node.get_clock().now().to_msg()

    for i, p in enumerate(pose_array.poses):
        target = PoseStamped()
        target.header.stamp = now
        target.header.frame_id = frame_id
        target.pose = p
        target.pose.position.z += z_offset

        # target.pose.orientation.x = 1.0
        # target.pose.orientation.y = 0.000651933
        # target.pose.orientation.z = 0.000017838
        # target.pose.orientation.w = 0.000445749

        targets.append(target)

        director.node.get_logger().info(
            f"[{i + 1}] above: x={target.pose.position.x:.3f}, "
            f"y={target.pose.position.y:.3f}, z={target.pose.position.z:.3f}"
        )

        tf_msg = TransformStamped()
        tf_msg.header.stamp = now
        tf_msg.header.frame_id = frame_id
        tf_msg.child_frame_id = f"detected_block_{i + 1}"

        tf_msg.transform.translation.x = target.pose.position.x
        tf_msg.transform.translation.y = target.pose.position.y
        tf_msg.transform.translation.z = target.pose.position.z

        tf_msg.transform.rotation = target.pose.orientation

        static_transforms.append(tf_msg)

    if static_transforms:
        static_broadcaster.sendTransform(static_transforms)
        director.node.get_logger().info(
            f"Published {len(static_transforms)} static TF frame(s)."
        )

    director.node.get_logger().info("Done with all targets.")

    rclpy.spin_once(director.node, timeout_sec=0.5)

    director.node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
