/* 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "gl19.h"
#include "sysenv.h"
#include "logger.h"
#include "shim/pbc_ext.h"
#include "sys/mem.h"

int gl19_sysenv_update(void *data) {
 
  /* if(!data) { */
  /*   LOG_EINVAL(&logger, __FILE__, "gl19_sysenv_update", __LINE__, LOGERROR); */
  /*   return IERROR; */
  /* } */

  /* sysenv->data = data; */

  return IOK;

}

void* gl19_sysenv_get() {
  return sysenv->data;
}

int gl19_sysenv_free() {

  /* if (sysenv->data) { */

  /*   pairing_clear(((gl19_sysenv_t *) sysenv->data)->pairing); */
  /*   pbc_param_clear(((gl19_sysenv_t *) sysenv->data)->param); */
  /*   mem_free(sysenv->data); sysenv->data = NULL; */

  /* } */
  
  return IOK;
  
}

/* env.c ends here */
