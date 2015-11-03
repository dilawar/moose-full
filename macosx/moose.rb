class Moose < Formula
  desc "Multiscale Object Oriented Simulation Environment"
  homepage "http://moose.ncbs.res.in"
  url "https://github.com/BhallaLab/moose-full/archive/master.tar.gz"
  version "3.0.2"

  depends_on "cmake" => :build
  depends_on "gsl"
  depends_on "homebrew/science/hdf5"
  depends_on "homebrew/science/libsbml" => :optional
  depends_on "matplotlib" => :python
  depends_on "numpy" => :python
  depends_on "python" if MacOS.version <= :snow_leopard
  depends_on "pyqt"

  def install
    mkdir "_build" do
      system "cmake", "..", *std_cmake_args
      system "make"
    end

    Dir.chdir("moose-core/python") do
      system "python", *Language::Python.setup_install_args(prefix)
    end

    lib.install "moose-gui"
    lib.install "moose-examples"

    # A wrapper script to launch moose gui.
    (bin/"moosegui").write <<-EOS.undent
      #!/bin/bash
      GUIDIR="#{lib}/moose-gui"
      (cd $GUIDIR && #{HOMEBREW_PREFIX}/bin/python mgui.py)
    EOS
    chmod 0755, bin/"moosegui"

    bin.env_script_all_files(libexec+"bin", :PYTHONPATH => ENV["PYTHONPATH"])
  end

  test do
    system "#{HOMEBREW_PREFIX}/bin/python", "-c", "import moose"
  end
end
