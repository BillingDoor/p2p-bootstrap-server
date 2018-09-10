# p2p-bootstrap-server
Typical TCP bootstrap server for a P2P botnet. Intended to provide new bots with node IPs to connect to to be introduced to the P2P network.

## About
This bootstrap server is a TCP server capable of accepting and interpreting requests made in JSON. It works by receiving IPs from the main nodes active in the P2P botnet and the IPs of the main nodes themselves in `std::vector<std::string>`'s. When a new bot becomes apart of the network, it requests a main node ip to connect to. There are two types of requests to handle this;

#### update_ips - Sent by main nodes in the P2P botnet.
```
{
  "auth":"p4AzO|L1jMf=",
  "request":"update_ips",
  "ips": {
	  "ip1":"144.12.123.11",
	  "ip2":"122.311.33.87",
	  "ip3":"91.122.31.633",
  },
  "ip_amount":3,
  "node_replaced":"61.13.119.1"
}
```

#### get_node - Sent by new bots requesting a node to connect to.
```
{
  "auth":"p4AzO|L1jMf=",
  "request":"update_ips"
}
```

### Requests explained
``auth`` - The auth part of the request is a simple token which is checked on the server, very, very poorly. I couldn't be bothered adding RSA encryption for communcations or for checking the integrity of the request, so this is a simple key that is validated by checking for certain parts of the string (found in ``client.cpp`` lines ``85-86``).

``ips`` - A simple list of IPs that can be as large as you'd like, they are dynamically saved and pushed into the IP vector.

``node_replaced`` - Sometimes in a P2P botnet, nodes die as the bot goes inactive. In the case of this happening, a new bot will need to replace it. When the node is replaced, the new node passes the old nodes IP address as a reference to as what to replace in the node vector.

## Disclaimer
I do not encourage the usage of this server for anything illegal, it was simply made for purpose of education.

## Libraries
<ul>
  <li>nlohmann - json</li>
</ul>
