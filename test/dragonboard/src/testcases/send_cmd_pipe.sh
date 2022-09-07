# the following functions use to comunication with core.
# example:
#   SEND_CMD_PIPE_OK $3
# or
#   SEND_CMD_PIPE_FAIL
#
# send message to core
SEND_CMD_PIPE_MSG()
{
    echo "$1 -1 $2" >> /tmp/cmd_pipe
}

# send OK to core
SEND_CMD_PIPE_OK()
{
    echo "$1 0" >> /tmp/cmd_pipe
}

# send OK with message to core
SEND_CMD_PIPE_OK_EX()
{
    echo "$1 0 $2" >> /tmp/cmd_pipe
}

# send Fail to core
SEND_CMD_PIPE_FAIL()
{
    echo "$1 1" >> /tmp/cmd_pipe
}

# send Fail with message to core
SEND_CMD_PIPE_FAIL_EX()
{
    echo "$1 1 $2" >> /tmp/cmd_pipe
}
