use crate::{
    fs::{
        file_system::FileSystem,
        inode::{Directory, FileLike},
    },
    result::Result,
};
use alloc::sync::Arc;
use unikernel_utils::once::Once;

use self::metrics::MetricsFile;

use super::tmpfs::TmpFs;

mod metrics;

pub static PROC_FS: Once<Arc<ProcFs>> = Once::new();
static METRICS_FILE: Once<Arc<dyn FileLike>> = Once::new();

/// Procfs is a virtual file system in Linux that exposes information about
/// running processes and system information in a hierarchical file-like structure.
/// It is typically mounted at /proc and provides access to a variety of information
/// about the system, including system hardware, kernel configuration,
/// and process details such as process ID, memory usage, and CPU usage.
/// The /proc file system is an important tool for system administrators
/// and developers for understanding and troubleshooting system issues.
pub struct ProcFs(TmpFs);

impl ProcFs {
    pub fn new() -> ProcFs {
        let tmpfs = TmpFs::new();
        let root_dir = tmpfs.root_tmpfs_dir();

        METRICS_FILE.init(|| Arc::new(MetricsFile::new()) as Arc<dyn FileLike>);

        root_dir.add_file("metrics", METRICS_FILE.clone());

        ProcFs(tmpfs)
    }
}

impl FileSystem for ProcFs {
    fn root_dir(&self) -> Result<Arc<dyn Directory>> {
        self.0.root_dir()
    }
}

pub fn init() {
    PROC_FS.init(|| Arc::new(ProcFs::new()));
}
