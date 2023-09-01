FROM python:3.10

RUN apt-get update && apt-get install -y python3-dev

RUN pip  install aiohttp==3.8.4 \
                 aiosignal==1.3.1 \
                 async-timeout==4.0.2 \
                 backoff==2.2.1 \
                 botocore==1.29.129 \
                 click==8.1.3 \
                 colorama==0.4.6 \
                 flask==2.2.3 \
                 frozenlist==1.3.3 \
                 gql==3.4.1 \
                 graphql-core==3.2.3 \
                 itsdangerous==2.1.2 \
                 Jinja2==3.1.2 \
                 jmespath==1.0.1 \
                 MarkupSafe==2.1.2 \
                 multidict==6.0.4 \
                 websockets==10.4 \
                 Werkzeug==2.2.3 \
                 yarl==1.9.2 \
                 deepclient==1.0.2 \
                 python-dotenv~=1.0.0 \
                 regex==2023.8.8 \
                 tiktoken==0.4.0 \
                 urllib3==1.26.16 \
                 librosa==0.10.1 \
                 transformers==4.32.1 \
                 torch==2.0.1

WORKDIR /app

COPY . /app

RUN chmod +x /app/cpp-docker-isolation-provider

CMD ["/app/cpp-docker-isolation-provider"]