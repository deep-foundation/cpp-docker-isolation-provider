# cpp-docker-isolation-provider

## Install/Build
```bash
pip install -r requirements.txt

apt-get install autoconf cmake make automake libtool git libboost-all-dev libssl-dev g++
#apt-get install libboost-python1.74-dev
cmake .
make
```

#### Or Local restart docker
```bash
docker build -t cpp-docker-isolation-provider .
docker run -d -p 39100:39100 -e PORT=39100 cpp-docker-isolation-provider
docker ps
```


## Check open ports
```bash
netstat -tuln
curl http://localhost:39100
```