#!/usr/bin/python
# -*-coding:Utf-8 -*

import argparse, subprocess

parser = argparse.ArgumentParser(description='Launch the P2P VC demo.')
parser.add_argument('users', type=int, help='Number of users')
parser.add_argument('--display', '-d', type=int, default=[], nargs='*', help='Users to display')
args=parser.parse_args()
if args.display == []:
  args.display = range(args.users)

for i in range(args.users):
  if i in args.display:
    subprocess.Popen(['../build/bin/video_conference', str(args.users), str(i)])
  else:
    subprocess.Popen(['../build/bin/video_conference', str(args.users), str(i), 'true'])
