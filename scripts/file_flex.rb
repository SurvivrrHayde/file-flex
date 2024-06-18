class File-Flex < Formula
  desc "file-flex"
  homepage "https://example.com"
  url "https://github.com/SurvivrrHayde/file-flex/releases/download/v1.0.0/file_flex.tar.gz"
  version "1.0.0"
  sha256 "b6e23f284aab6a8025255bf7463eb8fdab9ff6c3e57aa653c1406b313d1337b0"

  def install
    bin.install "bin/file_flex"
    bash_completion.install "scripts/file_flex-completion.sh"
  end

  test do
    system "#{bin}/file_flex", "help"
  end
end
