class CrossPlatformFsUtility < Formula
    desc "Cross-platform filesystem utility"
    homepage "https://example.com"
    url "https://example.com/cross_platform_fs_utility.tar.gz"
    version "1.0.0"
    sha256 "your_sha256_checksum_here"
  
    def install
      bin.install "cross_platform_fs_utility"
      bash_completion.install "cross_platform_fs_utility-completion.sh"
    end
  end
  