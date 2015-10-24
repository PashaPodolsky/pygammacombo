import os
import re
import subprocess

import megazord


class GenericCompiler:
    class ArgBuilder:
        def __str__(self):
            return ' '.join(self.flags)

        def __repr__(self):
            return ' '.join(self.flags)

        def __init__(self):
            self.flags = []

        def append(self, *flags):
            if isinstance(flags, list) or isinstance(flags, tuple):
                self.flags.extend(flags)
            else:
                self.flags.append(flags)
            return self

        def build(self):
            return self.flags

    def __init__(self, name, path=None):
        self.name = name
        if path is not None and os.path.isfile(path):
            self.path = path
        else:
            raise FileNotFoundError("{} was not found".format(path))

    def prepare_args(self, target):
        args = GenericCompiler.ArgBuilder()
        return args

    def compile(self, target):
        args = self.prepare_args(target)
        megazord.system.call(self.path, args.build())


class CCompiler(GenericCompiler):
    class CArgBuilder(GenericCompiler.ArgBuilder):
        def add_include_path(self, path):
            self.append('-I{}'.format(path))
            return self

        def add_include(self, name):
            self.append('-i{}'.format(name))
            return self

        def add_library_path(self, path):
            self.append('-L{}'.format(path))
            return self

        def add_library(self, name):
            self.append('-l{}'.format(name))
            return self

        def set_output_name(self, name):
            self.append('-o{}'.format(name))
            return self

        def set_std(self, std='c++11'):
            self.append('-std={}'.format(std))
            return self

        def set_target(self, sources, output_format):
            if output_format == '.o':
                self.append('-c')
            elif output_format in ['.so', '.dylib']:
                self.append('-shared')
            self.append(*sources)
            return self

        def build(self):
            return self.flags

    def prepare_args(self, target):
        args = self.CArgBuilder()
        args.set_target(target.get_sources(), target.output_format)
        compiled_lib_paths = []
        for dependency in target.dependencies:
            if dependency.output_format == '.o':
                args.append(dependency.output)
            elif dependency.output_format in ['.so', '.dylib']:
                compiled_lib_paths.append(dependency.output_dir)
                if dependency.output_name.startswith('lib'):
                    args.add_library(dependency.output_name[3:])
                else:
                    args.add_library(dependency.output_name)
            else:
                raise ValueError("{} cannot be processed as dependency for {}. "
                                 "Did you forget to set output format fro dependency to '.o'?".format(
                    dependency.sources))

        for compiled_lib_path in set(compiled_lib_paths):
            args.add_library_path(compiled_lib_path)

        if target.output_format in ['.so', '.dylib'] and not target.output.startswith('lib'):
            args.set_output_name(target.output_dir + 'lib' + target.output)
        else:
            args.set_output_name(target.output_dir + target.output)

        args.append('-O{}'.format(target.optimization_level))
        for library_path in target.library_paths:
            args.add_library_path(library_path)
        for include_path in target.include_paths:
            args.add_include_path(include_path)
        for library in target.libraries:
            args.add_library(library)
        for include in target.includies:
            args.add_include(include)
        return args

class DmdCompiler(GenericCompiler):
    def __init__(self, path=None):
        super(DmdCompiler, self).__init__("dmd", path)


class GccCompiler(CCompiler):
    def __init__(self, path=None):
        super(GccCompiler, self).__init__("gcc", path)


class GppCompiler(CCompiler):
    def __init__(self, path=None):
        super(GppCompiler, self).__init__("g++", path)


class ClangCompiler(CCompiler):
    def __init__(self, path=None):
        super(ClangCompiler, self).__init__("clang", path)


class ClangppCompiler(CCompiler):
    def __init__(self, path=None):
        super(ClangppCompiler, self).__init__("clang++", path)


class JarCompiler(GenericCompiler):
    class JarArgBuilder(GenericCompiler.ArgBuilder):
        def set_output_name(self, name):
            self.append('{}'.format(name))
            return self

        def set_target(self, sources):
            self.append(*sources)
            return self

    def __init__(self, path=None):
        super(JarCompiler, self).__init__("jar", path)

    def prepare_args(self, target):
        args = self.JarArgBuilder()
        if target.entry_point is None:
            args.append('cvf')
        else:
            args.append('cfe')
        args.set_output_name(target.output_dir + target.output)
        if target.entry_point is not None:
            args.append(target.entry_point)
        args.set_target(target.get_sources())
        return args

class JavaCompiler(GenericCompiler):
    class JavaArgBuilder(GenericCompiler.ArgBuilder):
        def add_classpath(self, classpath):
            self.append('-cp', classpath)
            return self

        def set_output_dir(self, dir):
            self.append('-d', dir)
            return self

        def set_std(self):
            return self

        def set_target(self, sources):
            self.append(*sources)
            return self

    def __init__(self, path=None):
        super(JavaCompiler, self).__init__("javac", path)

    def collect_classes(self, target):
        all_class_files = [f for f in os.listdir(target.output_dir) if
                           os.path.isfile(os.path.join(target.output_dir, f))]
        related_class_files = []
        sources_names = set(target.sources_names)
        for cf in all_class_files:
            if not cf.endswith('.class'):
                continue
            cf_crop = re.match("(\w+)[\$\.$]", cf).group(1)
            if cf_crop in sources_names:
                related_class_files.append(cf)
        return related_class_files

    def prepare_args(self, target):
        args = self.JavaArgBuilder()
        args.set_target(target.get_sources())
        args.set_output_dir(target.output_dir)
        dependencies = target.libraries
        for dependency in target.dependencies:
            if dependency.output_format != '.jar':
                raise ValueError("The only resolvable dependency is .jar")
            dependencies.append(dependency.output_dir + dependency.output)
        if len(dependencies) > 0:
            args.add_classpath(':'.join(dependencies))
        return args

    def compile(self, target):
        if target.output_format == '.jar':
            temp_dir = megazord.system.mkdtemp() + '/'
            old_output_dir = target.output_dir
            stored_wd = megazord.system.getwd()
            try:
                target.set_output_dir(temp_dir)
                args = self.prepare_args(target)
                megazord.system.call(self.path, args.build())
                megazord.system.info("auto-invoking jar tool...")
                collected_sources = self.collect_classes(target)
                megazord.system.setwd(temp_dir)
                jar_target = megazord.Target(collected_sources, output=target.output, entry_point=target.entry_point)
                jar_target.assembly()
            finally:
                megazord.system.setwd(stored_wd)
                megazord.system.move(temp_dir + target.output, old_output_dir + target.output)
                megazord.system.rm(temp_dir)

        else:
            args = self.prepare_args(target)
            megazord.system.call(self.path + " " + args.build())
