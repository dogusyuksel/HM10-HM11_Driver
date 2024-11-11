class Utils:
    def __init__(self, args):
        self.args = args

    def arg_check(self):
        if self.args.interface is None:
            print('interface argument is must')
            return False

        return True
