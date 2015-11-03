from wrapper import *
from wrapper.RooAbsArg import RooAbsArg
from wrapper.RooArgList import RooArgList

RooArgSet = mod.add_class('RooArgSet')
RooArgSet.add_constructor([param('const RooArgList&', 'list')])
RooArgSet.add_constructor([param('const RooArgSet&', 'set1'), param('const RooArgSet&', 'set2'),
                           param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const RooAbsArg&', 'var5'), param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const RooAbsArg&', 'var5'), param('const RooAbsArg&', 'var6'),
                           param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const RooAbsArg&', 'var5'), param('const RooAbsArg&', 'var6'),
                           param('const RooAbsArg&', 'var7'), param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const RooAbsArg&', 'var5'), param('const RooAbsArg&', 'var6'),
                           param('const RooAbsArg&', 'var7'), param('const RooAbsArg&', 'var8'),
                           param('const char*', 'name')])
RooArgSet.add_constructor([param('const RooAbsArg&', 'var1'), param('const RooAbsArg&', 'var2'),
                           param('const RooAbsArg&', 'var3'), param('const RooAbsArg&', 'var4'),
                           param('const RooAbsArg&', 'var5'), param('const RooAbsArg&', 'var6'),
                           param('const RooAbsArg&', 'var7'), param('const RooAbsArg&', 'var8'),
                           param('const RooAbsArg&', 'var9'), param('const char*', 'name')])

RooArgSet.add_method('getRealValue', retval('double'), [param('const char*', 'name'), param('double', 'defVal', default_value='0')])