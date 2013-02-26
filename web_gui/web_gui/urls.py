from django.conf.urls import patterns, include, url
from analyze.views import render_analyze
from browse.views import render_browseSamples, render_browseGroups, render_compareSamples, render_showSample, render_showGroup
from options.views import render_home, show_options, show_feedback

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'web_gui.views.home', name='home'),
    # url(r'^web_gui/', include('web_gui.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Admin:
    url(r'^jsejdak/admin/', include(admin.site.urls)),
    
    # Main pages:
    url(r'^jsejdak/home/$', render_home),
    url(r'^jsejdak/analyze/$', render_analyze),
    url(r'^jsejdak/browse_samples/$', render_browseSamples),
    url(r'^jsejdak/browse_groups/$', render_browseGroups),
    url(r'^jsejdak/options/$', show_options),
    url(r'^jsejdak/feedback/$', show_feedback),
    url(r'^jsejdak/compare/', render_compareSamples),
    url(r'^jsejdak/sample/', render_showSample),
    url(r'^jsejdak/group/', render_showGroup),
)
