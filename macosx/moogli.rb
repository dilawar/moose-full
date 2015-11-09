class Moogli < Formula
  desc "3d visualizer of neuronal simulation"
  homepage "http://moose.ncbs.res.in/moogli"
  url "https://github.com/BhallaLab/moogli/archive/master.tar.gz"
  version "0.5.0"

  resource "PyQt4" do
    url "http://sourceforge.net/projects/pyqt/files/PyQt4/PyQt-4.11.4/PyQt-mac-gpl-4.11.4.tar.gz"
    sha256 "f178ba12a814191df8e9f87fb95c11084a0addc827604f1a18a82944225ed918"
  end

  depends_on "open-scene-graph"
  depends_on "python" if MacOS.version <= :snow_leopard
  depends_on "gcc"

  def install
    resource("PyQt4").stage do
      system "python", "configure-ng.py", "--confirm-license"
      system "make"
    end

    ENV['CC'] = "#{HOMEBREW_PREFIX}/bin/gcc-5"
    ENV['CXX'] = "#{HOMEBREW_PREFIX}/bin/g++-5"
    system "python", "setup.py", "build"
  end

  test do
    system "#{HOMEBREW_PREFIX}/bin/python", "-c", "import moogli"
  end
end
