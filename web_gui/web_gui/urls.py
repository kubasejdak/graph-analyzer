from django.conf.urls import patterns, include, url
from analyze.views import show_analyze
from browse.views import show_browse
from options.views import show_home, show_options

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
    url(r'^jsejdak/$', show_home),
    url(r'analyze/$', show_analyze),
    url(r'browse/$', show_browse),
    url(r'options/$', show_options),
)
