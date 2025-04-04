docker run -d --name minio \
  -p 9000:9000 -p 9090:9090 \
  -e "MINIO_ROOT_USER=minioadmin" \
  -e "MINIO_ROOT_PASSWORD=minioadmin" \
  -v $(pwd)/data:/data \
  quay.io/minio/minio server /data --console-address ":9090"
