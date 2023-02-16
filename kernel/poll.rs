use crate::process::WaitQueue;
use unikernel_utils::once::Once;

pub static POLL_WAIT_QUEUE: Once<WaitQueue> = Once::new();

pub fn init() {
    POLL_WAIT_QUEUE.init(WaitQueue::new);
}
