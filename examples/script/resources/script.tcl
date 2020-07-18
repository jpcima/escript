hsize -id main 800 \
    [vtile \
         [button -size 2.0 -icon left -icon_pos right -body_color #337700 {Hello World}] \
         [top_margin 20 [xside_margin 100 [button Other]]] \
         [slider [basic_thumb 25] [slider_marks 20 [slider_labels 20 16.0 {L1 L2 L3} [basic_track 5]]]]]
