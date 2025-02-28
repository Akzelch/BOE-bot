#BOE-Bot
Collection of code for the SSY331 BOE-Bot assignment. The BOE-Bot examination consisted of four tasks, with the two most advanced ones included in this repository.

Task 3 involved an Arduino UNO mounted on two servo motors, with a ball at the back, forming a small two-wheeled robot. At the front, two whisker-like sensors were attached, short-circuiting their respective circuits to ground upon contact with an obstacle. This allowed the bot to determine whether it had collided on the left or right side. The code implements several maneuvers depending on whether the bot collides on the left, right, or directly in front. Additionally, a function was implemented to detect if the bot had gotten stuck in a corner.

A feature based on circular sector mathematics was also included to track the robot’s orientation, preventing it from turning around and driving back the way it came. The objective of the task was for the bot to navigate a course with various obstacles and walls on either side in the shortest possible time.

Task 4 used the same bot, but the whisker sensors were replaced with an ultrasonic sensor. The goal was to enable the bot to maintain a specified distance from an object, such as a hand or a wall. If the object moved forward or backward, the bot was required to adjust accordingly—for example, by reversing when the object moved closer.

