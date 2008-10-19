#!/usr/bin/env python

import unittest
from ecru import EcruPost
import os

class TestEcruPost(unittest.TestCase):
    
    def setUp(self):
        self.text = """subject: hello world
current_mood: alright
current_music: foo - bar
security: friendsonly

"""
        self.body = \
"""hello, how are you doing? it's a sample post, blah blah blah
i don't really know what bullshit to add there to make this
post a little bit longer than 2-3 lines. Ok, it's 3 lines,
ohoho, even 4 now, so I'll go back to the coding."""
        self.text += self.body
        
    def testEcruPost(self):
        ecruPost = EcruPost(self.text)

        props = ecruPost.props
        self.assert_(self.body == ecruPost.text)
        self.assert_("current_mood" in props)
        self.assert_("current_music" in props)
        self.assert_("security" in props)
        self.assert_(props["current_mood"] == "alright")
        self.assert_(props["current_music"] == "foo - bar")
        self.assert_(props["security"] == "friendsonly")

    def testWIthoutProps(self):
        txt = "foobar"
        ecruPost = EcruPost(txt)

        self.assert_(txt == ecruPost.text)

    def testThatDumpWorks(self):
        outputFilename = os.tmpnam()
        output = open(outputFilename, 'w+')
                
        ecruPost = EcruPost(self.text)
        ecruPost.dump(output)

        output.close()

        output = open(outputFilename, 'r')

        ecruPostDumped = EcruPost(''.join(output.readlines()))

        os.unlink(outputFilename)

        for i in ecruPost.props.keys():
            self.assert_(ecruPost.props[i] == ecruPostDumped.props[i])
        self.assert_(ecruPost.text == ecruPostDumped.text)            


if __name__ == "__main__":
    unittest.main()
