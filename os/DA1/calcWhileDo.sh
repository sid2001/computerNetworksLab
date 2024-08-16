#!/bin/bash

while true
do
    echo "Enter first number: "
    read num1
    echo "Enter second number: "
    read num2

    echo "Select operation: "
    echo "1. Addition"
    echo "2. Subtraction"
    echo "3. Multiplication"
    echo "4. Division"
    echo "5. Exit"

    read choice

    if [ "$choice" -eq 1 ]; then
        result=$((num1 + num2))
        echo "Result: $num1 + $num2 = $result"
    elif [ "$choice" -eq 2 ]; then
        result=$((num1 - num2))
        echo "Result: $num1 - $num2 = $result"
    elif [ "$choice" -eq 3 ]; then
        result=$((num1 * num2))
        echo "Result: $num1 * $num2 = $result"
    elif [ "$choice" -eq 4 ]; then
        if [ $num2 -ne 0 ]; then
            result=$((num1 / num2))
            echo "Result: $num1 / $num2 = $result"
        else
            echo "Error: Division by zero is not allowed."
        fi
    elif [ "$choice" -eq 5 ]; then
        echo "Exiting the calculator."
        break
    else
        echo "Invalid choice."
    fi

    echo ""
done
