#!/bin/sh
if [ -f interceptor ]; then
	rm interceptor
fi
if [ -f interceptor2 ]; then
	rm interceptor2
fi
if [ -f ori.log ]; then
	rm -f ori.log
fi
if [ -d target ]; then
	rm -rf target
fi

