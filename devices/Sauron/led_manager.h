/******************************************************************************
 * Copyright 2019 - Fausto Fusaro
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

void blink1() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  pinMode(LED_BUILTIN, INPUT);
}

void blink2() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  delay(75);
  pinMode(LED_BUILTIN, INPUT);
  delay(75);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(75);
  pinMode(LED_BUILTIN, INPUT);
}

void blink3() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  delay(50);
  pinMode(LED_BUILTIN, INPUT);
  delay(50);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(50);
  pinMode(LED_BUILTIN, INPUT);
  delay(50);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(50);
  pinMode(LED_BUILTIN, INPUT);
}

#endif //__LED_MANAGER_H__