#!/bin/bash
### BEGIN INIT INFO
# Provides:          rkaiq_3a
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description:
# Description:       Setup rkaiq 3a daemon
### END INIT INFO

start_3A()
{
    /usr/bin/rkaiq_3A_server 2>&1 | logger -t rkaiq &
}

stop_3A()
{
    killall rkaiq_3A_server || true
}


case "$1" in
  start)
    start_3A
    ;;
  stop)
    stop_3A
    ;;
  reload)
    stop_3A
    start_3A
    ;;
  *)
    echo "Usage: $0 {start|stop}"
    exit 1
    ;;
esac
exit 0
