

struct osd_state{
   enum state_t { external_video, internal_video, suspended};
   static void set(state_t state);
   static bool have_external_video(){return m_have_external_video == true;}
   static state_t get() { return m_current_state;}
   static void set_have_external_video(){m_have_external_video = true;}
 private:
   static void suspend();
   static void switch_to_internal_video_mode();
   static void switch_to_external_video_mode();
   static state_t m_current_state;
   static volatile bool m_have_external_video; 
};

