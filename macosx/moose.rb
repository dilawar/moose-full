class Moose < Formula
  desc "Multiscale Object Oriented Simulation Environment"
  homepage "http://moose.ncbs.res.in"
  url "https://github.com/BhallaLab/moose-full/archive/master.tar.gz"
  version "3.0.2"

  option "with-gui", "Enable gui support"

  depends_on "cmake" => :build
  depends_on "gsl"
  depends_on "hdf5"
  depends_on "libsbml" => :optional

  depends_on "matplotlib" => :python
  depends_on "numpy" => :python
  depends_on "python" if MacOS.version <= :snow_leopard

  if build.with?("gui")
    depends_on "pyqt"
  end

  def install
    mkdir "_build" do
      system "cmake", "..", *std_cmake_args
      system "make"
    end

    Dir.chdir("moose-core/python") do
      system "python", *Language::Python.setup_install_args(prefix)
    end

    if build.with?("gui")
      (lib/"moose").install "moose-gui"
      (lib/"moose").install "moose-examples"

      # A wrapper script to launch moose gui.
      (bin/"moosegui").write <<-EOS.undent
        #!/bin/bash
        GUIDIR="#{lib}/moose/moose-gui"
        (cd $GUIDIR && #{HOMEBREW_PREFIX}/bin/python mgui.py)
      EOS
      chmod 0755, bin/"moosegui"
    end

    bin.env_script_all_files(libexec+"bin", :PYTHONPATH => ENV["PYTHONPATH"])
  end

  def caveats
    <<-EOS.undent
    Please install python-networkx and python-suds-jurko using pip to complete
    the dependencies.

    $ pip install networkx suds-jurko
    EOS
  end

  test do
    # This will not work on Travis
    # system "#{HOMEBREW_PREFIX}/bin/python", "-c", "import moose"
    if build.with?("python")
      system "python", "-c", "import moose"
    end
  end
end
