from typing import Any, Optional, Union, Dict, List
from deepclient import DeepClient, DeepClientOptions
from gql import Client
from gql.transport.aiohttp import AIOHTTPTransport
from aiohttp.client_exceptions import ClientConnectorError
import asyncio

global_deep_client = None


def make_deep_client(token, url):
    global global_deep_client

    if global_deep_client is None:
        if not token:
            raise ValueError("No token provided")
        if not url:
            raise ValueError("No url provided")

        transport = AIOHTTPTransport(url=url, headers={'Authorization': f"Bearer {token}"})
        client = Client(transport=transport, fetch_schema_from_transport=True)
        options = DeepClientOptions(gql_client=client)
        global_deep_client = DeepClient(options)
        print("recreate")

    return global_deep_client


def select(token, url, exp: Union[Dict, int, List[int]], options: Dict = {}) -> dict | str:
    try:
        result = asyncio.run(make_deep_client(token, url).select(exp, options))
        return result
    except ClientConnectorError as e:
        return "Cannot connect to host"


def insert(token, url, objects, options: Dict = {}) -> dict | str:
    try:
        result = asyncio.run(make_deep_client(token, url).insert(objects, options))
        return result
    except ClientConnectorError as e:
        return "Cannot connect to host"


async def update(token, url, exp: Dict, value: Dict, options: Dict = {}) -> dict | str:
    try:
        result = asyncio.run(make_deep_client(token, url).update(exp, value, options))
        return result
    except ClientConnectorError as e:
        return "Cannot connect to host"


async def delete(token, url, exp: Union[Dict, int, List[int]], options: Dict = {}) -> dict | str:
    try:
        result = asyncio.run(make_deep_client(token, url).delete(exp, options))
        return result
    except ClientConnectorError as e:
        return "Cannot connect to host"


def serial(token, url, AsyncSerialParams: Dict):
    try:
        result = asyncio.run(make_deep_client(token, url).serial(AsyncSerialParams))
        return result
    except ClientConnectorError as e:
        return 'Cannot connect to host'


async def id(token, url, start: Any, *path: Any) -> int | str:
    try:
        result = asyncio.run(make_deep_client(token, url).id(start, path))
        return result
    except ClientConnectorError as e:
        return 'Cannot connect to host'


async def reserve(token, url):
    return await make_deep_client(token, url).reserve()


async def wait_for(token, url):
    return await make_deep_client(token, url).wait_for()


def id_local(token, url):
    return make_deep_client(token, url).id_local()


async def guest(token, url):
    return await make_deep_client(token, url).guest()


async def jwt(token, url):
    return await make_deep_client(token, url).jwt()


async def whoami(token, url):
    return await make_deep_client(token, url).whoami()


async def login(token, url):
    return await make_deep_client(token, url).login()


async def logout(token, url):
    return await make_deep_client(token, url).logout()


async def can(token, url):
    return await make_deep_client(token, url).can()


async def name(token, url):
    return await make_deep_client(token, url).name()


def name_local(token, url):
    return make_deep_client(token, url).name_local()
