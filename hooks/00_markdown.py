#!/usr/local/bin/python
#
# ecru hook implementing markdown markup support
# intended to be used as post-edit hook 
# place it to ~/.ecru/hooks/post and make sure it's
# executable bit it set
#
# author: Roman Bogorodskiy <bogorodskiy@gmail.com>
#

from ecru import EcruPost
import sys
import markdown

if len(sys.argv) != 2:
    sys.exit(1)

filename = sys.argv[1]

input = open(filename, 'r')

rawText = ''.join(input.readlines())
ecruPost = EcruPost(rawText)

input.close()

ecruPost.text = markdown.markdown(ecruPost.text)

output = open(filename, 'w')
ecruPost.dump(output)
output.close()
