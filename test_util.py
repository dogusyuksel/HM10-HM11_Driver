import argparse

import utils


def test_arg_check_filename():
    parser = argparse.ArgumentParser(
        prog='test',
        description='test desc',
        epilog='test epilog')
    parser.add_argument('-d', '--verbose',
                        action='store_true',
                        help='verbosity setting')
    parser.add_argument('-i', '--interface',
                        action='store',
                        help='serial interface name')
    args = parser.parse_args(['--verbose'])
    u = utils.Utils(args)

    assert u.arg_check() is False
