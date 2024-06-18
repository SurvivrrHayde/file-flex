class CrossPlatformFsUtility < Formula
  desc "Cross-platform filesystem utility"
  homepage "https://example.com"
  url "https://github.com/SurvivrrHayde/file-flex/releases/download/v1.0.0/file_flex.tar.gz"
  version "1.0.0"
  sha256 "b6e23f284aab6a8025255bf7463eb8fdab9ff6c3e57aa653c1406b313d1337b0"

  def install
    bin.install "bin/cross_platform_fs_utility"
    bash_completion.install "scripts/cross_platform_fs_utility-completion.sh"
  end

  test do
    system "#{bin}/cross_platform_fs_utility", "help"
  end
end
