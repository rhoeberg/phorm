"""Small example OSC client

This program sends 10 random values between 0.0 and 1.0 to the /filter address,
waiting for 1 seconds between each value.
"""
import argparse
import random
import time

import pythonosc
# from pythonosc.osc_message import OscMessage
# from pythonosc.osc_bundle import OscBundle

from pythonosc import udp_client


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("--ip", default="127.0.0.1",
      help="The ip of the OSC server")
  parser.add_argument("--port", type=int, default=5000,
      help="The port the OSC server is listening on")
  args = parser.parse_args()

  client = udp_client.SimpleUDPClient(args.ip, args.port)

  address = "/adr"
  builder = OscMessageBuilder(address)

  for x in range(3):
    r = random.random()
    client.send_message("/adr", r)
    print(r)
    time.sleep(1)


main()
