use std::sync::mpsc::Sender;

/// The Event type represents a pair of time and event_id.
pub type Event = (u64, u64);

struct Listener<M, H> {
    sender: Sender<M>,
    prev_hashes: Vec<H>,
}

pub struct DedupNotifier<M, H, F> {
    listeners: Vec<Listener<M, H>>,
    hasher: F,
}

impl<M, H, F> DedupNotifier<M, H, F>
where
    M: Send + Clone,
    H: PartialEq + Clone,
    F: Fn(M) -> H,
{
    pub fn new(hasher: F) -> Self {
        Self {
            hasher,
            listeners: Vec::new(),
        }
    }

    pub fn add_listener(&mut self, notifier: Sender<M>) {
        self.listeners.push(Listener {
            sender: notifier,
            prev_hashes: Vec::new(),
        });
    }

    pub fn notify(&mut self, message: M) {
        let hash = (self.hasher)(message.clone());
        self.listeners.iter_mut().for_each(|listener| {
            if listener.prev_hashes.contains(&hash) {
                return;
            } else {
                listener.prev_hashes.push(hash.clone());
            }
            listener.sender.send(message.clone()).unwrap();
        });
    }
}

