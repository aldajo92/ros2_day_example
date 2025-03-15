#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class RandomPublisher : public rclcpp::Node
{
public:
    RandomPublisher()
        : Node("random_publisher")
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        timer_ = this->create_wall_timer(
            500ms, std::bind(&RandomPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto message = geometry_msgs::msg::Twist();

        // Generar valores aleatorios para velocidad lineal y angular
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> linear_dist(-2.0, 2.0);
        std::uniform_real_distribution<float> angular_dist(-2.0, 2.0);

        message.linear.x = linear_dist(gen);   // Velocidad lineal aleatoria
        message.angular.z = angular_dist(gen); // Velocidad angular aleatoria

        RCLCPP_INFO(this->get_logger(), "Publishing: linear=%.2f, angular=%.2f",
                    message.linear.x, message.angular.z);

        publisher_->publish(message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RandomPublisher>());
    rclcpp::shutdown();
    return 0;
}
