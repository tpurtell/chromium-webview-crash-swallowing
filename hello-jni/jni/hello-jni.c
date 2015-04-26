#include <string.h>
#include <jni.h>

extern int hook_signal(int i);

void
Java_com_example_hellojni_HelloJni_sigsegv( JNIEnv* env,
                                                  jobject thiz )
{
   *(int*)0 = 0xdeadbeaf;
}

void
Java_com_example_hellojni_HelloJni_hook( JNIEnv* env,
                                                  jobject thiz, jint sig )
{
   hook_signal(sig);
}
