import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import time

# Sample data for cars
cars = [
    {
        "model": "CarA",
        "currentSpeed": 60,  # kmph
        "brakePressure": 30,  # Psi
        "tirePressure": 32,  # Psi
        "abs": "Yes",
        "ebd": "Yes",
        "esc": "Yes",
        "airbags": 4,
        "frictionCoefficient": 0.8,
        "stoppingDistance": 30,  # meters
        "maxImpact": 5000  # Newtons
    },
    {
        "model": "CarB",
        "currentSpeed": 80,
        "brakePressure": 25,
        "tirePressure": 28,
        "abs": "No",
        "ebd": "Yes",
        "esc": "No",
        "airbags": 2,
        "frictionCoefficient": 0.6,
        "stoppingDistance": 50,
        "maxImpact": 7000
    }
]

# Function to calculate safety score
def calculate_safety_score(car, speed, friction_coefficient):
    score = 100 - (speed / 10) - (car["maxImpact"] / 10) + (car["airbags"] * 2) + (friction_coefficient * 10)
    if car["abs"] == "Yes":
        score += 5
    if car["ebd"] == "Yes":
        score += 3
    if car["esc"] == "Yes":
        score += 4
    return round(score)

# Data for real-time plotting
time_data = []
safety_scores = []

# Select a car for simulation
selected_car = cars[0]  # You can allow dynamic selection in the final version

# Real-time update function
def update(frame):
    global time_data, safety_scores

    # Simulating changing speed and recalculating safety score
    updated_speed = selected_car["currentSpeed"] + np.sin(frame / 10) * 5
    updated_score = calculate_safety_score(selected_car, updated_speed, selected_car["frictionCoefficient"])

    # Updating data
    time_data.append(frame)
    safety_scores.append(updated_score)

    # Clear old data if too much
    if len(time_data) > 100:
        time_data.pop(0)
        safety_scores.pop(0)

    # Update plot
    ax.clear()
    ax.plot(time_data, safety_scores, label="Safety Score")
    ax.set_title(f"Real-Time Safety Score Simulation for {selected_car['model']}")
    ax.set_xlabel("Time (arbitrary units)")
    ax.set_ylabel("Safety Score")
    ax.legend()
    ax.grid()

# Plot setup
fig, ax = plt.subplots()
ani = animation.FuncAnimation(fig, update, interval=100)

plt.show()
