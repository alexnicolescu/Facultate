echo "001122334455" | sed -e 's|[0-9A-F][0-9A-F]|&:|g' -e 's|:$||'