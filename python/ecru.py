__author__ = "Roman Bogorodskiy <bogorodskiy@gmail.com>"

class EcruPost(object):
    text = ''
    props = {}

    def __init__(self, text):
        self.parseText(text)

    def parseText(self, text):
        lines = text.split('\n')
        readingText = False

        if lines[0].find(":") == -1:
            self.text = '\n'.join(lines)
            return

        for i in range(0, len(lines)):
            if len(lines[i].strip()) == 0:
                self.text = '\n'.join(lines[i+1:])
                return
            elif lines[i].find(":") != -1:
                idx = lines[i].find(":")
                propName = lines[i][0:idx].strip()
                propValue = lines[i][idx+1:].strip()
                self.props[propName] = propValue

    def dump(self, output):
        for propName, propValue in self.props.iteritems():
            output.write('%s: %s\n' % (propName, propValue))
                    
        output.write('\n')
        output.write(self.text)
