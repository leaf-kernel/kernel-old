#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

// CPU related headers
#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>

// x86_64 architecture specific headers
#include <arch/x86_64/acpi/mcfg.h>
#include <arch/x86_64/idt/idt.h>

// File system related headers
#include <fs/vfs.h>

// Memory management related headers
#include <libc/stdlib/memory/kheap.h>

// System configuration headers
#include <sys/_config.h>

// Backtrace related headers
#include <sys/backtrace.h>

// Real-time clock related headers
#include <sys/time/rtc.h>

// Utility headers
#include <utils/convertion.h>

int main() {
    update_memory();

    if(total_memory < 64000000)
        plog_warn("Your computer only has %dMB of RAM. Leaf "
                  "recommends atleast 64MB!\033[0m",
                  bytes_to_mb(total_memory));
    else
        plog_ok("%d bytes OK", total_memory);
    rtc_time_point time = rtc_get();

    plog_ok("%.3s %.3s %.2d %.2d:%.2d:%.2d @ tty%03d",
            _get_day(time.day_of_week), _get_month(time.month),
            time.day_of_month, time.hours, time.minutes, time.seconds,
            currentTTYid);

#ifdef __LEAF_GAY_SEX__
    plog_ok(
        "\033[1mGay Sex\033[0m: Nathan was in his dorm, stroking his hard 9 "
        "inch when his dormmate Matthew came in. Nate gasped, trying to cover "
        "himself, but Matthew wasn't having any of that. He locked the door to "
        "Nate's bedroom and walked over, taking his clothes off and leaving "
        "them on the floor on the way. \"What are you-\" Nate began before "
        "being pinned to the bed, lips locked with Matt's. Matt looked around. "
        "\"Got any lube?\" He murmered softly. Nate shook his head before "
        "passing him a wrapper. Matt wondered, and then threw it, taking Nate "
        "by surprise and fucking him rough, cumming immediately into him to "
        "lube him up. He had been rock-hard anyways, his masturbation session "
        "in the locker room had helped with that. Nate moaned loudly as Matt "
        "fucked him missionary, slowing down and becoming softer in his "
        "thrusts. Nate wrapped his legs around Matt, already breaking a sweat. "
        "Matt grinned, stopping and slowly going deeper into him with those "
        "magnificent 11 inches until Nate was gasping, clutching the bed. Matt "
        "then began to slowly fuck him again. Nate began to squirm under his "
        "roommate when the pressure broke and he couldn't stop himself "
        "cumming. Matthew stared in surprise before spanking him and making "
        "him shiver. \"Lick all this off you slut!\" Nate obeyed, and they "
        "were soon flat on the bed frenching again, before Nate took Matt in "
        "his hand, rubbing until his massive cock was completely hard and red. "
        "Matthew wasted no time pulling away and insead of cumming into Nate's "
        "hand, fucked him and squirted inside him. Nate moaned loudly kids "
        "from other dorms heard it, launching into a large orgasm. He was "
        "panting when he'd finished, the sweat dripping off of his lean body. "
        "Matt smiled, beginning to pound him again, causing Nate to bite his "
        "shoulder so hard Matt yelled out in pain and ecstacy. After thier "
        "wonderful sex, they both showered, wore thier pyjamas and cuddled on "
        "the sofa watching a random movie until they both fell asleep. What "
        "lovely Gay Sex.");
#endif

    hlt();
    return LEAF_RETURN_SUCCESS;
}
