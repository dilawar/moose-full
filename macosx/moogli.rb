class Moogli < Formula
  desc "3d visualizer of neuronal simulation"
  homepage "http://moose.ncbs.res.in/moogli"
  url "https://github.com/BhallaLab/moogli/archive/master.tar.gz"
  version "0.5.0"

  depends_on "cmake" => :build
  depends_on "python" if MacOS.version <= :snow_leopard
  depends_on "open-scene-graph"
  depends_on "sip"
  depends_on "pyqt"
  depends_on "moose"

  def install
    system "cmake", ".", *std_cmake_args
    system "make"
    system "python", "moogli/cmake_modules/setup.py", "install", \
        "--prefix=#{prefix}", "--record=installed.txt"
  end

  test do
    system "python", "-c", "import moogli"
  end
end
