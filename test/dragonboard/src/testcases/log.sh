LOGE()
{
	printf "$(date "+%m-%d %H:%M:%S") %5d E %-10s: \033[48;31m$*\033[0m\n" $$ ${LOG_TAG:0:10}
}

LOGW()
{
	printf "$(date "+%m-%d %H:%M:%S") %5d W %-10s: \033[48;33m$*\033[0m\n" $$ ${LOG_TAG:0:10}
}

LOGI()
{
	printf "$(date "+%m-%d %H:%M:%S") %5d I %-10s: \033[48;32m$*\033[0m\n" $$ ${LOG_TAG:0:10}
}

LOGD()
{
	printf "$(date "+%m-%d %H:%M:%S") %5d D %-10s: \033[48;34m$*\033[0m\n" $$ ${LOG_TAG:0:10}
}
