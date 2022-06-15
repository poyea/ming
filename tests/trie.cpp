#include "gtest/gtest.h"

#include <ming/trie.hpp>

namespace trie_test {

class Trie_TEST : public ::testing::Test {
protected:
  Trie_TEST() = default;

  virtual ~Trie_TEST() {}

  virtual void SetUp() {
    // post-construction
  }

  virtual void TearDown() {
    // pre-destruction
  }

  ming::Trie trie;
};

TEST_F(Trie_TEST, TrieSearch) {
  trie.insert("word");
  EXPECT_TRUE(trie.search("word"));
  EXPECT_TRUE(not trie.search("word_not_exist"));
  EXPECT_TRUE(not trie.search("not_exist"));
}

TEST_F(Trie_TEST, TriePrefix) {
  trie.insert("word");
  EXPECT_TRUE(trie.starts_with("wo"));
  EXPECT_TRUE(not trie.starts_with("po"));
}

} // namespace trie_test