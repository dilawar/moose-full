class Moogli < Formula
  desc "3d visualizer of neuronal simulation"
  homepage "http://moose.ncbs.res.in/moogli"
  url "https://github.com/BhallaLab/moogli/archive/master.tar.gz"
  version "0.5.0"

  depends_on "open-scene-graph"
  depends_on "python" if MacOS.version <= :snow_leopard
  depends_on "pyqt"
  depends_on "gcc"

  def install
    ENV['CC'] = 'gcc'
    ENV['CXX'] = 'g++'
    system "python", *Language::Python.setup_install_args(prefix)
  end

  test do
    system "#{HOMEBREW_PREFIX}/bin/python", "-c", "import moogli"
  end
end
