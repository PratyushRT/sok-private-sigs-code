file(REMOVE_RECURSE
  "../../lib/libgroupsig-static.a"
  "../../lib/libgroupsig-static.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/groupsig-static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
