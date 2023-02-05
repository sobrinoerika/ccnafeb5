>>TASK 1: Fundamentals of cisco security
20% of the exam : acl, nat. ssh, dhcp
ssh: secure shell = match better than telnet

@how to enable ssh on COREBABA
conf t
aaa new-model
username admin privilege 15 secret password
ip domain-name rivanit.com
crypto key generate rsa
ip ssh version 2
end

>>TASK 2: para bilis
@COREBABA
conf t
no aaa new-model
Line vty 0 14
 no login

>>TASK 3: Acces-list: control who gets access to your devices
allow only your pc to connect.
@COREBABA acl 1-99
conf t
no access-list 1
access-list 1 permit host 10.31.1.10
access-list 1 permit host 10.31.100.8
line vty 0 14
access-class 1 in
end

@COREBABA (for website GUI)
conf t
ip http server
ip http secure-server
end

http://10.31.1.4

How to restrict access to website
ip http access-class 1

>>TASK 4: how to protect your network security
1. checkpoint
2. palo alto
3. fortinet
4. cisco firepower
5. Juniper srx

>>step 1. how to be a great cyber cecurity engineer.
@make CUCM intentionnaly hackable
conf t
service finger
service tcp-small-servers
service udp-small-servers
ip dns server
ip http server
ip http secure-server
ip host cm 10.31.100.8
ip host cm2 10.31.100.9
ip host pc 10.31.1.10
ip host p1 10.31.100.101
ip host p2 10.31.100.102
interface Fa 0/0
 ip address  10.31.100.9 255.255.255.0 Secondary
 end

>>TASK5: Make your own packet filter firewall
 ex 1: make a packet filter firewall that will allow this policy
 1. cm only open http, https
 2. cm2 only open dns, ssh
 formula "protocol:tcp, udp, icmp, ip hacker: victim, port )

@CUCM
conf t
no ip access-list extended PFW1
ip access-list extended PFW1
permit tcp any host cm eq 80
permit tcp any host cm eq 2000
permit tcp any host cm2 eq 53
permit tcp any host cm2 eq 22
int fa 0/0
 ip access-group PFW1 in
 do sh access-list PFW1
 end

>>EX2: make firewall policy 3 that will allow these
 1. cm will allow ping(icmp),sccp, dns, ssh 
 2. cm2 will allow https
@CUCM
conf t
no ip access-list extended PFW2
ip access-list extended PFW2
permit icmp any host cm
permit tcp any host cm eq 2000
permit tcp any host cm eq 53
permit tcp any host cm eq 22
permit tcp any host cm2 eq 443
int fa 0/0
 ip access-group PFW2 in
 end

>>Ex3: construct FP3 that will meet:
 1. cm allows ssh, telnet, dns
 2. cm2 allows ping, http, https, dns
@CUCM
conf t
no ip access-list extended PFW3
ip access-list extended PFW3
permit tcp any host cm eq 22
permit tcp any host cm eq 23
permit tcp any host cm eq 53
permit icmp any host cm2
permit tcp any host cm2 eq 80
permit tcp any host cm2 eq 443
permit tcp any host cm2 eq 53
int fa 0/0
 ip access-group PFW3 in
 end

>>EX4:pfw4 
 cm allows ping, 80
 cm2, ping, 53
 @CUCM
conf t
no ip access-list extended PFW4
ip access-list extended PFW4
permit tcp any host cm eq 80
permit icmp any host cm
permit icmp any host cm2
permit tcp any host cm2 eq 53
int fa 0/0
 ip access-group PFW4 in
 end

>>EX5:multiple hackers; multiple victims
 make a policy fwp5 
 1. pc allow ping, 80, on cm
 2. pc allow telnet on cm2
 3. p1 allow sccp on cm
 4. p2 allow ping on cm2
@CUCM
conf t
no ip access-list extended PFW5
ip access-list extended PFW5
permit icmp host pc host cm
permit tcp host pc host cm eq 80
permit tcp host pc host cm2 eq 23
permit tcp host p1 host cm eq 2000
permit icmp host p2 host cm2
int fa 0/0
 ip access-group PFW5 in
 end

>>EX6:multiple hackers; multiple victims
 make a policy fwp5 
 1. pc allow telnet on cm
 2. pc allow ping on cm2
 3. p1 allow ping on cm
 4. p2 allow sccp on cm
@CUCM
conf t
no ip access-list extended PFW6
ip access-list extended PFW6
permit tcp host pc host cm eq 23
permit icmp host pc host cm2
permit icmp host p1 host cm
permit tcp host p2 host cm eq 2000
int fa 0/0
 no ip access-group PFW6 in
 end

>>TASK7: the 4th laboratory on exam
 NETWROK TIME PROTOCOL:

 @EDGE: NTP master/timekeeper
 conf t
 ntp master
 ntp source gi 0/0/0
 int gi 0/0/0
  ntp broadcast
end

@COREBABA/ @CUCM
conf t
ntp server 10.31.31.1
end
show clock

>>TASK8: learning the 4 laboratory in exam
LAB1:give all ip address
@NATPROXY
conf t
int gi 1
ip add 192.168.108.8 255.255.255.0
no shut
int gi 3
ip add 192.168.102.8 255.255.255.0
no shut
ip route 0.0.0.0 0.0.0.0 192.168.108.2
ip domain-lookup
ip name-server 1.1.1.1
end

>>TASK9: make cisco a internet server using NAT
step 1: create an access list to allow:
gi 2 192.168.101.8 and gi 3 192.168.102.8

config t
no access-list 1 
access-list 1 permit 192.168.101.0 0.0.0.255
access-list 1 permit 192.168.102.0 0.0.0.255
do sh access-list 1

Step2: define outside: give internet inside:get/received internet

conf t
int gi 1
ip nat outside
int gi 2
ip nat inside
int gi 3
ip nat inside
end

Step 3: creat a nat dynamic and static pool with overload
@dynamic: give internet automatically.
conf t
ip nat inside source list 1 interface gi 1 overload

@Static NAT entries
conf t
ip nat inside source static 192.168.101.8 192.168.108.69
ip nat inside source static 192.168.102.8 192.168.108.88
do show ip nat translations
end

Inside global           inside local        outside local    outside global
Give NAT IP address      Getprivate IP      www.fb.com       www.fb.com      

>>TASK10: DHCP Server with LINUX Clients
@NATPROXY
config t
ip dhcp excluded-add 192.168.101.1 192.168.101.150
ip dhcp excluded-add 192.168.102.1 192.168.102.150
ip dhcp pool G2
 network 192.168.101.0 255.255.255.0
 default-router 192.168.101.8
 domain-name ccnaexam.com
 dns-server 1.1.1.1
ip dhcp pool G3
 network 192.168.102.0 255.255.255.0
 default-router 192.168.102.8
 domain-name ccnaexam.com
 dns-server 1.1.1.1

 NTP: google
 conf t
 ip domain-lookup
 ip name-server 1.1.1.1
 ntp server time.google.com
 end
 show ntp associations

>>TASK11: blocking
config t
no access-list 2
access-list 2 deny 66.254.114.0 0.0.0.255
access-list 2 deny 192.124.249.0 0.0.0.255
access-list 2 deny 23.40.252.0 0.0.0.255
access-list 2 deny 162.211.82.0 0.0.0.255
access-list 2 deny 104.22.2.0 0.0.0.255
access-list 2 permit any
int gi 1
 ip access-group 2 in
end
 
TASK 12: SSH in CSR 1000v
do nothing, auto run




