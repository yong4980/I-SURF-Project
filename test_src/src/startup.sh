sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor"

sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor"

sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor"

sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor"

sudo bash -c "echo userspace > /sys/devices/system/cpu/cpu5/cpufreq/scaling_governor"

sudo bash -c "echo 1 > /sys/devices/system/cpu/cpu1/online"

sudo bash -c "echo 1 > /sys/devices/system/cpu/cpu2/online"

sudo bash -c "echo 1 > /sys/devices/system/cpu/cpu3/online"

sudo bash -c "echo 1 > /sys/devices/system/cpu/cpu4/online"

sudo bash -c "echo 1 > /sys/devices/system/cpu/cpu5/online"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq"

sudo bash -c "echo 2035200 > /sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq"

sudo bash -c "echo 345600 > /sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq"
