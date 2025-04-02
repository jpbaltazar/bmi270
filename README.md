# BMI270 APP

## Setup

Create a virtual python environment

    python -m pip venv .venv

Source it:

    source .venv/bin/activate

Start the skeleton with west

    west init -m <repo> --manifest-rev main

Configure the board

    west config build.board native_sim

Synchronize the west workspace. This might take while.

    west update

## Build

Move to the application folder 'app'

    cd app

Build the project

    west build

## Run

Run the app from the 'app' folder

    ./zephyr/zephyr.exe
