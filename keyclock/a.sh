1. docker立ち上げ
```
docker run -p 8082:8080 -e KEYCLOAK_ADMIN=admin -e KEYCLOAK_ADMIN_PASSWORD=admin quay.io/keycloak/keycloak:25.0.1 start-dev
```
2. 管理者でログイン
http://localhost:8082

3. ユーザー作成
4. Client(アプリ)の作成
5. ログイン

curl http://localhost:8082/auth/realms/myrealm/protocol/openid-connect/token -d "grant_type=password&client_id=test&client_secret=ezGFgQA2e9OSOJLb2UauDdSSYBHgietH&username=myuser&password=myuser&scope=openid"