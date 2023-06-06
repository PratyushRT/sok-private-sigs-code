file(REMOVE_RECURSE
  "../../lib/libgroupsig.pdb"
  "../../lib/libgroupsig.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/groupsig.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
