#!/bin/bash


for i in {1..10}
do
	for j in {1..2}
        do
                result=$((i * j))
                echo -n "$i * $j = $result  "
        done

	echo ""
done

