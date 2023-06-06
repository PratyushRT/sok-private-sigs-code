#include <groupsig/groupsig.h>
#include <time.h>

int main (int argc, char *argv[]) {

  groupsig_key_t *mgrkey;
  groupsig_key_t *grpkey;
  gml_t *gml;
  groupsig_key_t **memkey;
  uint32_t n;
  groupsig_signature_t *sig;
  message_t *msg;
  uint8_t b;
  message_t *m0, *m1, *m2, *m3, *m4;
  int t;
  double time_spent = 0;
  int rc;
  int group_size = 32;
  /* Initialize environment for GL19 */
  rc = groupsig_init(GROUPSIG_PS16_CODE, 0);

  /** Create the group **/

  /* Initialize the group and manager keys */
  grpkey = groupsig_grp_key_init(GROUPSIG_PS16_CODE);
  mgrkey = groupsig_mgr_key_init(GROUPSIG_PS16_CODE);
  gml = gml_init(GROUPSIG_PS16_CODE);
  memkey = NULL;
  n=0;

  /* Run the setup */
  rc = groupsig_setup(GROUPSIG_PS16_CODE, grpkey, mgrkey, gml);
  /** Add a member to the group **/

  /* Initialize the member key */
  memkey = (groupsig_key_t **) malloc(sizeof(groupsig_key_t *)*group_size);
    
  /* Simulate the issue-join interactive process */
  m0 = m1 = m2 = m3 = m4 = NULL;
  int i;
  for (i=0; i<group_size; i++) {

	memkey[i] = groupsig_mem_key_init(GROUPSIG_PS16_CODE);

	m1 = message_init();
	rc = groupsig_join_mgr(&m1, gml, mgrkey, 0, m0, grpkey);
	m2 = message_init();
    rc = groupsig_join_mem(&m2, memkey[i], 1, m1, grpkey);
	m3 = message_init();
	rc = groupsig_join_mgr(&m3, gml, mgrkey, 2, m2, grpkey);
	m4 = message_init();
    rc = groupsig_join_mem(&m4, memkey[i], 3, m3, grpkey);

  }
  if(m1) { message_free(m1); m1 = NULL; }
  if(m0) { message_free(m0); m0 = NULL; }
  if(m2) { message_free(m2); m2 = NULL; }
  if(m3) { message_free(m3); m3 = NULL; }
  if(m4) { message_free(m4); m4 = NULL; }

  /** Sign a message **/

  /* Prepare the message */
  msg = message_from_string("Hello, World!");
  
  /* Initialize the signature object */
  for(t=0; t < 10; t++) {
  clock_t begin = clock();
  sig = groupsig_signature_init(GROUPSIG_PS16_CODE); 
  rc = groupsig_sign(sig, msg, memkey[0], grpkey, UINT_MAX);
  clock_t end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  }
  time_spent = time_spent/10;
  fprintf(stdout, "Signing time: %f\n", time_spent);

  /** Verify the signature **/
  groupsig_verify(&b, sig, msg, grpkey);

  if (b) {
    fprintf(stdout, "VALID signature.\n");
  } else {
    fprintf(stdout, "WRONG signature.\n");
  }

  /* Free memory */
  groupsig_clear(GROUPSIG_PS16_CODE);
  groupsig_grp_key_free(grpkey); grpkey = NULL;
  groupsig_mgr_key_free(mgrkey); mgrkey = NULL;
  gml_free(gml); gml = NULL;
  if (memkey) {
	for (i=0; i<group_size; i++) {
	  groupsig_mem_key_free(memkey[i]); memkey[i] = NULL;
	}
	free(memkey); memkey = NULL;
  }
  groupsig_signature_free(sig); sig = NULL;
  message_free(msg); msg = NULL;
  message_free(m1); m1 = NULL;
  message_free(m2); m2 = NULL;
  message_free(m3); m3 = NULL;
  message_free(m4); m4 = NULL;
  message_free(m4); m0 = NULL;
  
  return 0;
  
}
