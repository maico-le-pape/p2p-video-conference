#!/usr/bin/python
# -*-coding:Utf-8 -*

import argparse, random, subprocess

parser = argparse.ArgumentParser(description="Define random delays in a p2p simulation.")
parser.add_argument('--min', type=int, help='Minimum delay', default=50)
parser.add_argument('--max', type=int, help='Maximum delay', default=150)
parser.add_argument('users',type=int, help='Number of users')
args = parser.parse_args()

# Assign various delays
delays = [ [ random.randint(args.min, args.max) for i in range(args.users) ] for j in range(args.users) ]
 
# Make matrix symmetric
for i in range(args.users):
	for j in range(i):
		delays[i][j] = delays[j][i]

for i in range(args.users):
	delays[i][i] = 0

# Create qdisc tree
with open('/dev/null', 'w') as f:
	subprocess.call(['tc', 'qdisc', 'del', 'dev', 'lo', 'root'], stdout=f, stderr=subprocess.STDOUT)
bands = max(args.users, 3)
subprocess.call(['tc', 'qdisc', 'add', 'dev', 'lo', 'root', 'handle', '1:', 'prio', 'bands', str(bands)])
for i in range(args.users):
	subprocess.call(['tc', 'qdisc', 'add', 'dev', 'lo', 'parent', '1:' + str(i + 1), 'handle', str(i + 2) + ':', 'prio', 'bands', str(bands)])
	subprocess.call(['tc', 'filter', 'add', 'dev', 'lo', 'protocol', 'ip', 'parent', '1:', 'prio', '1', 'u32', 'match', 'ip', 'sport', str(10000 + i), '0xffff', 'flowid', '1:' + str(i + 1)])

for i in range(args.users):
	for j in range(args.users):
		subprocess.call(['tc', 'qdisc', 'add', 'dev', 'lo', 'parent', str(i + 2) + ':' + str(j + 1), 'handle', str(args.users * (i + 1) + j + 2) + ':', 'netem', 'delay', str(delays[i][j]) + 'ms'])
		subprocess.call(['tc', 'filter', 'add', 'dev', 'lo', 'protocol', 'ip', 'parent', str(i + 2) + ':', 'prio', '1', 'u32', 'match', 'ip', 'dport', str(10000 + j), '0xffff', 'flowid', str(i + 2) + ':' + str(j + 1)])

for line in delays:
	print(line)
