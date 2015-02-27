
Index_

=========================================
craft, features and widgets.
=========================================

   Craft
         Could be one of:

            + aircraft
            + watercraft
            + land vehicle
            + amphibious vehicle
            + submarine
            + etc

   Features
         Variables representing fine grained aspects of the crafts behaviour/properties/state.
         Some examples of Features of a craft. A craft may or may not have 
         a particular feature [#craft_feature]_ .

            + current time
            + current latitude
            + current longtitude
            + current altitude 
            + current pitch
            + current yaw
            + current roll
            + batteries
               - so ... Number of batteries, name per battery, capacity per battery, voltage etc?

   Widgets
         Widgets are designed based on the features they require to work [#widget_robust]_ .

            * MyCompassWidget    - requires feature current yaw 
            * MyHomeArrowWidget - requires features current latitude,current Longitude
            * MyAltitudeWidget   - requires feature Altitude ( yes really :) )
            * MyArtificialHorizonWidget - requires current pitch and current roll.

   global-feature-list
         A global list of features available, with comprehensive definitions
 
   user-feature-list 
         The features of the users craft, selected from the global list of
         features.

   available-widget-list
         A set of widgets that are available for use.
         May comprise Widgets that have been accumulated from various sources
         or created by the user or both.

   user-widget-list 
         The available-widget-list is filtered by the user-feature-list to select
         only those that use features on the user-feature-list [#filter_override]_ :

   user-firmware
         The user firmware is generated from the user-widget-list. 

Index_

.. _Features: http://www.wikipedia.org/wiki/Feature_(machine_learning)
.. _Index: index.html

.. [#craft_feature]  A craft with a GPS can be assumed to have latitude, longtitude, altitude etc.
.. [#widget_robust]  Widgets must be designed to be able to cope with failed requests for data.
.. [#filter_override]  It should be possible to override the filter, so a user can test some feature. 



