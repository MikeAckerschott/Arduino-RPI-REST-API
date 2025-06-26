#!/bin/bash

# Check for at least 2 arguments
if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    echo "Usage: $0 \"METHOD /endpoint\" [\"body\"] \"IP PORT\""
    echo "Example: $0 \"PUT /config/mode\" \"active\" \"192.168.2.3 80\""
    exit 1
fi

METHOD_ENDPOINT="$1"
BODY="${2:-}" # Default to an empty string if not provided
IP_PORT="${@: -1}" # Get the last argument as IP and PORT

# Calculate content length if body is provided
if [ -n "$BODY" ]; then
    CONTENT_LENGTH=${#BODY}
    BODY_PART="\r\nContent-Length: $CONTENT_LENGTH\r\n\r\n$BODY"
else
    BODY_PART="\r\n\r\n"
fi

# Build and send raw HTTP request
printf "$METHOD_ENDPOINT HTTP/1.1$BODY_PART" | nc $IP_PORT
