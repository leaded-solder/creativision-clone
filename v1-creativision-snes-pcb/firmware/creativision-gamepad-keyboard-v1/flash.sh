#!/bin/bash
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program creativision-gamepad-keyboard-v1.elf verify reset exit"
