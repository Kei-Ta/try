FROM ubuntu:latest
# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y build-essential sudo libcgroup-dev

# 作業ディレクトリを設定
WORKDIR /mnt

# コンテナが起動する際のデフォルトコマンド
CMD ["bash"]