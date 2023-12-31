FROM python:3.10

RUN apt-get update && apt-get install -y python3-dev

RUN pip3 install aiohttp==3.8.4 \
                 aiosignal==1.3.1 \
                 async-timeout==4.0.2 \
                 backoff==2.2.1 \
                 botocore==1.29.129 \
                 frozenlist==1.3.3 \
                 gql==3.4.1 \
                 graphql-core==3.2.3 \
                 jmespath==1.0.1 \
                 multidict==6.0.4 \
                 websockets==10.4 \
                 deepclient==1.0.2 \
                 yarl==1.9.2

WORKDIR /app

COPY . /app

RUN chmod +x /app/cpp-docker-isolation-provider

ENV LD_LIBRARY_PATH=.

CMD ["/app/cpp-docker-isolation-provider"]